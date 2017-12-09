var path = require('path');
var dir = '../test/specs/';

[
    'fastText'
].forEach((script) => {
    require(path.join(dir, script));
});