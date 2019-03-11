var mqtt = require('mqtt');
var client  = mqtt.connect('<Broker Address>');

client.on('connect', function () {
    client.subscribe('<sub_topic>', function (err) {
        if (!err) {
            client.publish('init', 'Server Online');
        }
    })
});

client.on('message', function (topic, message) {
    // message is Buffer
    handleMessage(topic, message);
});

function handleMessage(topic, message) {

    var msg = message.toString();
    console.log(msg);

    client.end()
}
