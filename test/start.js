var path = require('path');
var dir = '../test/specs/';

[
    'fastText',
    'langid',
].forEach((script) => {
    require(path.join(dir, script));
});