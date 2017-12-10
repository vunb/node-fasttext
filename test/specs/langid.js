'use strict';
const test = require('tape');
const path = require('path');
const fastText = require('../../index');

test('fastText language identification', function (t) {
    t.plan(4)
    
    let model_path = path.resolve(path.join(__dirname, '../models/lid.176.ftz'))
    console.log('File model path: ' + model_path)
    let classifier = new fastText.Classifier(model_path);

    classifier.predict('sử dụng vntk với fastext rất tuyệt?', 5, (err, res) => {
        t.ok(!err, 'no errors')
        t.equal(res.length, 5, 'number of classifications output')
        t.equal(res[0].label, '__label__vi', 'output is __label__vi');
        t.true(res[0].value > 0.99, 'confidence is 99%');
        console.log('Result: ', err, res);
     });
})