var binary = require('node-pre-gyp');
var path = require('path')

var binaryPath = binary.find(path.resolve(path.join(__dirname, './package.json')));
var FastText = require(binaryPath);

module.exports = FastText;