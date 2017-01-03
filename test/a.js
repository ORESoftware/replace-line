const addon = require('../build/Release/hello.node');

const val = addon.hello('/home/oleg/dogs.txt','\\S+', "false");

const util = require('util');

console.log('val => \n', util.inspect(val));