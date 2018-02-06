var path = require('path');
var dir = '../test/specs/';

[
    'fastText',
    'langid',
    'trainer',
].forEach((script) => {
    require(path.join(dir, script));
});