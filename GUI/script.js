
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

let rooms = []

document.addEventListener('DOMContentLoaded', () => {
    
    document.querySelectorAll(".room").forEach(item => {

        let room_id = item.getAttribute('room-id')

        rooms[room_id] = {
            element : item,
            room_id : room_id,
            CO2_level_generator : simulate_CO2(0.05, (Math.random() * (1100 - 900)) + 900, 50 ),
            temp_generator : simulate_temperature(22.0, 1.0)
        }

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

    let interval = setInterval(async ()=>{
        data = ""

        for(let room of rooms){

            let co2_level = room.CO2_level_generator.next().value
            let temp = room.temp_generator.next().value
            data += `${room.room_id};0;${co2_level}\n${room.room_id};2;${temp}\n`
        
        }

        res = await fetch('http://localhost:8082',{
            headers : {
                'Content-Type' : 'text/plain'
            },
            method : 'POST',
            body : data
        })

        res = await fetch('http://localhost:8085')

        data = await res.json()

        
        for(i=0;i<data.size;i++){

            if(!data.rooms[i].light)
                rooms[i].element.querySelector('img.light').style.display = "none"
            else
                rooms[i].element.querySelector('img.light').style.display = "inline"

            
            if(!data.rooms[i].ventilation)
                rooms[i].element.querySelector('img.ventilation').style.display = "none"
            else
                rooms[i].element.querySelector('img.ventilation').style.display = "inline"

            
            if(!data.rooms[i].ac)
                rooms[i].element.querySelector('img.air-conditioner').style.display = "none"
            else
                rooms[i].element.querySelector('img.air-conditioner').style.display = "inline"

            if(!data.rooms[i].presence)
                rooms[i].element.querySelector('img.presence').style.display = "none"
            else
                rooms[i].element.querySelector('img.presence').style.display = "inline"

        }

        console.log(data)

    }, 1000)
})
