const addon = require('../build/Release/hello.node');

const val = addon.hello('/home/oleg/dogs.txt',['\\S+'], "true", 7,3,90);

const util = require('util');

console.log('val => \n', util.inspect(val));