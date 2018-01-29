'use strict';
const test = require('tape');
const path = require('path');
const fastText = require('../../index');

test('fastText trainer', function (t) {
    t.plan(1)

    let data = path.resolve(path.join(__dirname, '../data/cooking.train.txt'));
    let model = path.resolve(path.join(__dirname, '../data/cooking.model'));
    let trainer = new fastText.Trainer();
    let options = {
        input: data,
        output: model,
        loss: "softmax",
        dim: 200,
        bucket: 2000000
    }

    trainer.train('supervised', options)
        .then((res) => {
            t.equal(res.dim, 100, 'dim')
        });
})