var addon = require('./build/Release/hello.node');


const val = addon.hello();

// val.forEach(function(a){
//    console.log(a);
// });
console.log(' => hello => ', val);