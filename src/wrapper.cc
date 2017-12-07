


#include "wrapper.h"

#include <math.h>

#include <iostream>
#include <sstream>
#include <iomanip>
#include <thread>
#include <string>
#include <vector>
#include <queue>
#include <algorithm>

using fasttext::model_name;
using fasttext::entry_type;

constexpr int32_t FASTTEXT_VERSION = 12; /* Version 1b */
constexpr int32_t FASTTEXT_FILEFORMAT_MAGIC_INT32 = 793712314;

Wrapper::Wrapper(std::string modelFilename)
    : quant_(false),
        modelFilename_(modelFilename),
        isLoaded_(false),
        isPrecomputed_(false) {}

void Wrapper::getVector(Vector& vec, const std::string& word) {
    const std::vector<int32_t>& ngrams = dict_->getSubwords(word);
    vec.zero();
    for (auto it = ngrams.begin(); it != ngrams.end(); ++it) {
        vec.addRow(*input_, *it);
    }
    if (ngrams.size() > 0) {
        vec.mul(1.0 / ngrams.size());
    }
}

bool Wrapper::checkModel(std::istream& in) {
    int32_t magic;
    int32_t version;
    in.read((char*)&(magic), sizeof(int32_t));
    if (magic != FASTTEXT_FILEFORMAT_MAGIC_INT32) {
        return false;
    }
    in.read((char*)&(version), sizeof(int32_t));
    if (version != FASTTEXT_VERSION) {
        return false;
    }
    return true;
}

void Wrapper::signModel(std::ostream& out) {
    const int32_t magic = FASTTEXT_FILEFORMAT_MAGIC_INT32;
    const int32_t version = FASTTEXT_VERSION;
    out.write((char*)&(magic), sizeof(int32_t));
    out.write((char*)&(version), sizeof(int32_t));
}

void Wrapper::loadModel() {
    if (isLoaded_) {
        return;
    }
    mtx_.lock();
    if (isLoaded_) {
        mtx_.unlock();
        return;
    }
    std::ifstream ifs(this->modelFilename_, std::ifstream::binary);
    if (!ifs.is_open()) {
        throw "Model file cannot be opened for loading!";
    }
    if (!checkModel(ifs)) {
        throw "Model file has wrong file format!";
    }
    loadModel(ifs);
    ifs.close();
    isLoaded_ = true;
    mtx_.unlock();
}

void Wrapper::loadModel(std::istream& in) {
    args_ = std::make_shared<Args>();
    dict_ = std::make_shared<Dictionary>(args_);
    input_ = std::make_shared<Matrix>();
    output_ = std::make_shared<Matrix>();
    qinput_ = std::make_shared<QMatrix>();
    qoutput_ = std::make_shared<QMatrix>();
    args_->load(in);

    dict_->load(in);

    bool quant_input;
    in.read((char*) &quant_input, sizeof(bool));
    if (quant_input) {
        quant_ = true;
        qinput_->load(in);
    } else {
        input_->load(in);
    }

    in.read((char*) &args_->qout, sizeof(bool));
    if (quant_ && args_->qout) {
        qoutput_->load(in);
    } else {
        output_->load(in);
    }

    model_ = std::make_shared<Model>(input_, output_, args_, 0);
    model_->quant_ = quant_;
    model_->setQuantizePointer(qinput_, qoutput_, args_->qout);

    if (args_->model == model_name::sup) {
        model_->setTargetCounts(dict_->getCounts(entry_type::label));
    } else {
        model_->setTargetCounts(dict_->getCounts(entry_type::word));
    }
}

void Wrapper::precomputeWordVectors() {
    if (isPrecomputed_) {
        return;
    }
    precomputeMtx_.lock();
    if (isPrecomputed_) {
        precomputeMtx_.unlock();
        return;
    }
    Matrix wordVectors(dict_->nwords(), args_->dim);
    wordVectors_ = wordVectors;
    Vector vec(args_->dim);
    wordVectors_.zero();
    for (int32_t i = 0; i < dict_->nwords(); i++) {
        std::string word = dict_->getWord(i);
        getVector(vec, word);
        real norm = vec.norm();
        wordVectors_.addRow(vec, i, 1.0 / norm);
    }
    isPrecomputed_ = true;
    precomputeMtx_.unlock();
}

std::vector<PredictResult> Wrapper::findNN(const Vector& queryVec, int32_t k,
        const std::set<std::string>& banSet) {

    real queryNorm = queryVec.norm();
    if (std::abs(queryNorm) < 1e-8) {
        queryNorm = 1;
    }
    std::priority_queue<std::pair<real, std::string>> heap;
    Vector vec(args_->dim);
    for (int32_t i = 0; i < dict_->nwords(); i++) {
        std::string word = dict_->getWord(i);
        real dp = wordVectors_.dotRow(queryVec, i);
        heap.push(std::make_pair(dp / queryNorm, word));
    }

    PredictResult response;
    std::vector<PredictResult> arr;
    int32_t i = 0;
    while (i < k && heap.size() > 0) {
        auto it = banSet.find(heap.top().second);
        if (it == banSet.end()) {
            response = { heap.top().second, exp(heap.top().first) };
            arr.push_back(response);
            i++;
        }
        heap.pop();
    }
    return arr;
}

std::vector<PredictResult> Wrapper::nn(std::string query, int32_t k) {
    Vector queryVec(args_->dim);
    std::set<std::string> banSet;
    banSet.clear();
    banSet.insert(query);
    getVector(queryVec, query);
    return findNN(queryVec, k, banSet);
}

std::vector<PredictResult> Wrapper::predict (std::string sentence, int32_t k) {

    std::vector<PredictResult> arr;
    std::vector<int32_t> words, labels;
    std::istringstream in(sentence);

    dict_->getLine(in, words, labels, model_->rng);

    // std::cerr << "Got line!" << std::endl;

    if (words.empty()) {
        return arr;
    }

    Vector hidden(args_->dim);
    Vector output(dict_->nlabels());
    std::vector<std::pair<real,int32_t>> modelPredictions;
    model_->predict(words, k, modelPredictions, hidden, output);

    PredictResult response;

    for (auto it = modelPredictions.cbegin(); it != modelPredictions.cend(); it++) {
        response = { dict_->getLabel(it->second), exp(it->first) };
        arr.push_back(response);
    }

    return arr;
}
