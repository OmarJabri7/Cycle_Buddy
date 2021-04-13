import pyrebase
from PIL import Image
img = Image.open("src/img.jpg")
img.save("src/img.ppm")
config = {"apiKey": "AIzaSyCwYWvvDBfqsazPMXNhRIRX0HJ7RIsEH_c",
          "authDomain":"cyclebuddy-17c84.firebaseapp.com",
          "databaseURL": "https://cyclebuddy-17c84-default-rtdb.firebaseio.com/",
          "projectId":"cyclebuddy-17c84",
          "storageBucket":"cyclebuddy-17c84.appspot.com",
          "messagingSenderId":"703404260627"}
firebase = pyrebase.initialize_app(config)
storage = firebase.storage()
storage.child("images/img.ppm").put("src/img.ppm")