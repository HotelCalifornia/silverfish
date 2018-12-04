const AWS = require('aws-sdk');
AWS.config.Region = 'us-east-1';

const iotdata = new AWS.IotData({ endpoint: 'acobcyfsx523y-ats.iot.us-east-2.amazonaws.com' });

exports.handler = async (event) => {
    console.log(event.Records[0].Sns);
    for (let r of event.Records) {
        const msg = JSON.parse(r.Sns.Message);
        switch (msg.action) {
        case 'drive':
            await iotdata.publish({
                topic: 'robot/drive',
                payload: JSON.stringify({
                    left_power: msg.payload.power,
                    right_power: msg.payload.power,
                    duration: msg.payload.duration
                }),
                qos: 0
            }).promise();
            break;
        case 'turn':
            if (!msg.direction) return { statusCode: 400, message: 'must specify a direction!' }
            if (msg.direction !== 'cw' && msg.direction !== 'ccw') return { statusCode: 400, message: `invalid direction ${msg.direction}!` }
            let lp;
            let rp;
            switch (msg.direction) {
                case 'cw':
                   lp = -msg.payload.power;
                   rp = msg.payload.power;
                   break;
                case 'ccw':
                    lp = msg.payload.power;
                    rp = -msg.payload.power;
                    break;
            }
            await iotdata.publish({
                topic: `robot/drive`,
                payload: JSON.stringify({
                    left_power: lp,
                    right_power: rp,
                    duration: msg.payload.duration
                }),
                qos: 0
            }).promise();
            break;
    }
    }
    return { statusCode: 200, message: 'finished successfully' }
};
