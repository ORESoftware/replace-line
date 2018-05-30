'use strict';

let values = [
    {
        num:1,
        char:'a'
    },
    {
        num:2,
        char:'b'
    },
    {
        num:1,
        char:'c'
    }
];



// values = values.sort(function(left,right){
//      return  true;
// });

values = values.sort(function(left,right){
    return  left.num > right.num && left.char > right.char;
});


console.log(values);