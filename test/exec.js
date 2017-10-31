describe('Exec', function () {
  it('should emit radio signal', function (done) {
        var exec = require('../lib/exec.js');
        exec({
            deviceType: {
                identifier: '7289728'
            },
            state: {
                value: 1
            }
        })
        .then(() => done())
  });
});
