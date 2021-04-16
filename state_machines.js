var fsm = require('fsm-as-promised');
fsm({
initial: 'start',
final: ['end'],
events:[
    {name:'Waiting for mobile connection',from:'start', to:'sensors'}
],
});