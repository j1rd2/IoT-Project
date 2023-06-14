window.addEventListener('load', (event) => {
    // Your web app's Firebase configuration
    var firebaseConfig = {
        apiKey: "AIzaSyBdNJ82PipRL0HfpVw5sqidBaZv65xWNOA",
        authDomain: "tc1004b-48578.firebaseapp.com",
        databaseURL: "https://tc1004b-48578-default-rtdb.firebaseio.com",
        projectId: "tc1004b-48578",
        storageBucket: "tc1004b-48578.appspot.com",
        messagingSenderId: "510427288538",
        appId: "1:510427288538:web:f16e35a454faaef0c966e3",
        measurementId: "G-ZJGXK9HFJV"
      };

    // Initialize Firebase
    firebase.initializeApp(firebaseConfig);

    var database = firebase.database();

    document.getElementById('myButton').addEventListener('click', function() {
        var ref = database.ref('/');

        ref.on('value', function(snapshot) {
            document.getElementById('myData').textContent = JSON.stringify(snapshot.val(), null, 2);
        });
    });
});
