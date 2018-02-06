'use strict';
const test = require('tape');
const path = require('path');
const fastText = require('../../index');

test('fastText trainer', function (t) {
    t.plan(1)

    let data = path.resolve(path.join(__dirname, '../data/cooking.train.txt'));
    let model = path.resolve(path.join(__dirname, '../data/cooking.model'));
    let classifier = new fastText.Classifier();
    let options = {
        input: data,
        output: model,
        loss: "softmax",
        dim: 200,
        bucket: 2000000
    }

    classifier.train('supervised', options)
        .then((res) => {
            console.log('model info after training:', res)
            t.equal(res.dim, 200, 'dim')
        });
})