
function* simulate_CO2(build_up_rate, initial_CO2_level, room_volume){
    let current_CO2_level = initial_CO2_level
    const increment_per_second = (build_up_rate * room_volume)/60

    while(true){
        yield current_CO2_level.toFixed(2)
        current_CO2_level += increment_per_second;
    }
}

function* simulate_temperature(initial_temp, fluctuation_range){
    let current_temp = initial_temp

    while(true){
        const fluctuation = Math.random() * fluctuation_range - fluctuation_range / 2
        current_temp += fluctuation
        yield current_temp.toFixed(2);
    }
}

const intervals = []


document.addEventListener('DOMContentLoaded', () => {
    
    document.querySelectorAll(".room").forEach(item => {

        let room_id = item.getAttribute('room-id')
        const CO2_level_generator = simulate_CO2(0.05, (Math.random() * (500 - 400)) + 400, 50 )
        const temp_generator = simulate_temperature(22.0, 1.0);


        let interval = setInterval(()=>{
            let co2_level = CO2_level_generator.next().value
            let temp = temp_generator.next().value
            fetch('http://localhost:8082',{
                headers : {
                    'Content-Type' : 'text/plain'
                },
                method : 'POST',
                body : `${room_id};0;${co2_level}\n${room_id};2;${temp}`
            })
        }, 1000)
        intervals.push(interval)

        item.addEventListener('mouseenter', event => {
            fetch('http://localhost:8082',{
                headers : {
                    'Content-Type' : 'text/plain'
                },
                method : 'POST',
                body : `${room_id};1;1.0`
            })
        })
        item.addEventListener('mouseleave', event => {
            let room_id = item.getAttribute('room-id')
            fetch('http://localhost:8082',{
                headers : {
                    'Content-Type' : 'text/plain'
                },
                method : 'POST',
                body : `${room_id};1;0.0`
            })
        })
    })
})
