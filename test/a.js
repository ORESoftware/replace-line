const addon = require('../build/Release/run.node');

const val = addon.run('/home/oleg/dogs.txt',['\\S+'], "true", 7,3,90);

const util = require('util');

console.log('val => \n', util.inspect(val));