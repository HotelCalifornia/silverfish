const AWS = require('aws-sdk');
AWS.config.Region = 'us-east-1';

const iotdata = new AWS.IotData({ endpoint: 'acobcyfsx523y-ats.iot.us-east-2.amazonaws.com' });

exports.handler = async (event) => {
    switch (event.action) {
        case 'drive':
            console.log(`driving at ${event.payload.power} for ${event.payload.duration}ms`)
            await iotdata.publish({
                topic: 'robot/drive',
                payload: JSON.stringify(event.payload),
                qos: 0
            }).promise();
            break;
        case 'turn':
            if (!event.direction) return { statusCode: 400, message: 'must specify a direction!' }
            if (event.direction !== 'cw' && event.direction !== 'ccw') return { statusCode: 400, message: `invalid direction ${event.direction}!` }
            await iotdata.publish({
                topic: `robot/turn/${event.direction}`,
                payload: JSON.stringify(event.payload),
                qos: 0
            }).promise();
            break;
    }
    return { statusCode: 200, message: 'finished successfully' }
};
