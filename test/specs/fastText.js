'use strict';
const test = require('tape');
const path = require('path');
const fastText = require('../../index');

test('fastText classifier', function (t) {
    t.plan(3)
    
    let model_path = path.resolve(path.join(__dirname, '../models/model_cooking.bin'))
    console.log('File model path: ' + model_path)
    let classifier = new fastText.Classifier(model_path);

    classifier.predict('Why not put knives in the dishwasher?', 5, (err, res) => {
        t.ok(!err, 'no errors')
        t.equal(res.length, 5, 'number of classifications output')
        t.equal(res[0].label, '__label__knives', 'output is __label__knives');
        console.log('Result: ', err, res);
     });
})

test('fastText classifier load model', function (t) {
    t.plan(4)
    
    let model_path = path.resolve(path.join(__dirname, '../models/model_cooking.bin'))
    console.log('File model path: ' + model_path)
    let classifier = new fastText.Classifier();

    classifier.loadModel(model_path).then((info) => {
        console.log('load model success!!!', info);
        t.equal(info.model, 'supervised');
    });

    classifier.predict('Why not put knives in the dishwasher?', 5, (err, res) => {
        t.ok(!err, 'no errors')
        t.equal(res.length, 5, 'number of classifications output')
        t.equal(res[0].label, '__label__knives', 'output is __label__knives');
        console.log('Result: ', err, res);
     });
})