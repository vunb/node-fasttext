'use strict';
const test = require('tape');
const path = require('path');
const fastText = require('../../index');

test('fastText classifier', function (t) {
    t.plan(3)

    let model_path = path.resolve(path.join(__dirname, '../models/model_cooking.bin'))
    console.log('File model path: ' + model_path)
    let classifier = new fastText.Classifier();

    classifier.loadModel(model_path)
        .then((info) => {
            console.log('load model success!!!', info);
            t.equal(info.model, 'supervised', 'load supervised model');
            return classifier.predict('Why not put knives in the dishwasher?', 5, 1);
        })
        .then((res) => {
            t.equal(res.length, 5, 'number of classifications output')
            t.equal(res[0].label, '__label__knives', 'output is __label__knives');
        })
        .catch((err) => {
            console.log('Result: ', err);
            t.fail(err);
        })
})