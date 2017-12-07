

#ifndef WRAPPER_H
#define WRAPPER_H

// #include <time.h>

#include <atomic>
#include <memory>
#include <set>
#include  <mutex>

#include "../fastText/src/fasttext.h"

using fasttext::Args;
using fasttext::Dictionary;
using fasttext::Matrix;
using fasttext::QMatrix;
using fasttext::Model;
using fasttext::Vector;
using fasttext::real;

struct PredictResult {
    std::string label;
    double value;
};

class Wrapper {
    private:
        std::shared_ptr<Args> args_;
        std::shared_ptr<Dictionary> dict_;

        std::shared_ptr<Matrix> input_;
        std::shared_ptr<Matrix> output_;

        std::shared_ptr<QMatrix> qinput_;
        std::shared_ptr<QMatrix> qoutput_;

        std::shared_ptr<Model> model_;
        Matrix wordVectors_;

        // std::atomic<int64_t> tokenCount;
        // clock_t start;

        void signModel(std::ostream&);
        bool checkModel(std::istream&);

        std::vector<PredictResult> findNN(const Vector&, int32_t,
                    const std::set<std::string>&);

        void loadModel(std::istream&);

        bool quant_;
        std::string modelFilename_;
        std::mutex mtx_;
        std::mutex precomputeMtx_;

        bool isLoaded_;
        bool isPrecomputed_;
    public:
        Wrapper(std::string modelFilename);

        void getVector(Vector&, const std::string&);

        std::vector<PredictResult> predict(std::string sentence, int32_t k);
        std::vector<PredictResult> nn(std::string query, int32_t k);

        void precomputeWordVectors();
        void loadModel();

};

#endif
