import React, { useEffect, useState } from 'react';
import fbapp from '../api/firebaseConfig'; 
import moment from 'moment';
import { getDatabase, ref, onValue } from "firebase/database";
import { Line } from 'react-chartjs-2';
import { Chart, registerables } from 'chart.js';
Chart.register(...registerables);

function Graph() {
  const [data, setData] = useState([]);

  useEffect(() => {
    const fetchData = async () => {
      const db = getDatabase(fbapp);
      const dataref = ref(db, 'Sensor');
      onValue(dataref, (snapshot) => {
        const newData = [];
        snapshot.forEach((childSnapshot) => {
          const dataVal = childSnapshot.val();
          newData.push({
            date: moment.unix(dataVal.epoch).format('h:mm A'),
            temperature: dataVal.temp,
            humidity: dataVal.hum,
          });
        });
        setData(newData);
      });
    };

    fetchData();
  }, []);

  const chartData = {
    labels: data.map(item => item.date),
    datasets: [
      {
        label: 'Temperature',
        data: data.map(item => item.temperature),
        borderColor: 'rgba(255, 99, 132, 1)',
        backgroundColor: 'rgba(255, 99, 132, 0.2)',
      },
      {
        label: 'Humidity',
        data: data.map(item => item.humidity),
        borderColor: 'rgba(54, 162, 235, 1)',
        backgroundColor: 'rgba(54, 162, 235, 0.2)',
      }
    ]
  };

  return (
    <div style={{ width: '70%'}}>
      <h2>Data Visualization</h2>
      <Line data={chartData} />
    </div>
  );
}

export default Graph;
