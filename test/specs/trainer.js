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

test('fastText quantize', function (t) {
  t.plan(1)
  let input = path.resolve(path.join(__dirname, '../data/cooking.train.txt'));
  let output = path.resolve(path.join(__dirname, '../data/cooking.model'));
  let classifier = new fastText.Classifier();
  let options = {
    input,
    output,
    epoch: 1,
    qnorm: true,
    qout: true,
    retrain: true,
    cutoff: 1000,
  };

  classifier.train('quantize', options)
    .then((res) => { console.log(res) })
    .catch((e) => { console.error(e) });

  t.ok(true);
})
