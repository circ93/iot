// element DOM
let acc_x = document.getElementById('acc_x');
let acc_y = document.getElementById('acc_y');
let acc_z = document.getElementById('acc_z');

let final_sma = document.getElementById('SMA');
let counter_sma = document.getElementById('counter_sma');
let average_sma = document.getElementById('average_sma');

let activity_average = document.getElementById('activity_average');
let activity_realTime = document.getElementById('activity_realTime');

let error = document.getElementById('error');

let samples_counter = 0;
let sma_counter = 0;
let sma_data = 0;

var user_state;
var x_sensor;
var y_sensor;
var z_sensor;

if ( 'Accelerometer' in window ) {
    let sensor = new LinearAccelerationSensor({ referenceFrame: 'device', frequency: 1 });

    sensor.addEventListener('reading', function (e) {
        // real-time accelerometer values
        x_sensor = e.target.x;
        y_sensor = e.target.y;
        z_sensor = e.target.z;
        acc_x.innerHTML = x_sensor;
        acc_y.innerHTML = y_sensor;
        acc_z.innerHTML = z_sensor;

        samples_counter ++;

        // check the number of samples generated
        if (samples_counter === 5) {
            // I calculate the SMA
            let sma = Math.abs(e.target.x) + Math.abs(e.target.y) + Math.abs(e.target.z);

            // previous SMA sum
            sma_data += sma;
            // increment counter SMA
            sma_counter += 1;

            // view counter samples
            counter_sma.innerHTML = '<br>Samples ' + sma_counter +'/12';

            // I calculate partial average SMA
            var partial_average_sma = sma_data / sma_counter;
            average_sma.innerHTML = sma;

            // Activity real time
            if (sma < 1.5) {
                activity_realTime.innerHTML = "still";
            }
            if (sma > 1.5 && sma < 4) {
                activity_realTime.innerHTML = "walking";
            }
            if (sma > 4) {
                activity_realTime.innerHTML = "running";
            }

            // sample counter reset
            samples_counter = 0;
        }

        // check the number of SMA generated
        if (sma_counter === 12) {
            // final average calculation
            let activity = sma_data / 12;

            final_sma.innerHTML = activity;

            // user activity control
            if (activity < 1.5) {
                user_state = "still";
            }
            if (activity > 1.5 && activity < 4) {
                user_state = "walking";
            }
            if (activity > 4){
                user_state = "running";
            }
            activity_average.innerHTML = user_state;

            sma_counter = 0;
            // delete the SMA values calculated previously
            sma_data = 0;
            sma_data_2 = 0;
        }
    });

    sensor.addEventListener('error', event => {
        console.log(event.error.name, event.error.message);
    });
    sensor.start();
}
else error.innerHTML = 'Accelerometer not supported';