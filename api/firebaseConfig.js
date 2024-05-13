import { initializeApp } from "firebase/app";

const firebaseConfig = {
  apiKey: "AIzaSyChuxFIjmjICbbTGT1XOFtxRmrlkWw53TM",
  authDomain: "iot69-f4b58.firebaseapp.com",
  databaseURL: "https://iot69-f4b58-default-rtdb.firebaseio.com",
  projectId: "iot69-f4b58",
  storageBucket: "iot69-f4b58.appspot.com",
  messagingSenderId: "325506266673",
  appId: "1:325506266673:web:727114a6c7ba4dc35ac710"
};

// Initialize Firebase
const fbapp= initializeApp(firebaseConfig);

export default fbapp;