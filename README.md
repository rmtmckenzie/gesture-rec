gesture-rec
===========

This project started off as a capstone project for a software engineering degree at the University of Victoria. The general idea was to take the output of any webcam or other camera, and use that for gesture recognition.

There are many specialized devices in the marketplace for gesture recognition which are able to perform with a high degree of accuracy and precision; however, most consumers do not have access to this level of technology yet. What the average person does have is a laptop with a webcam, or a mobile phone with two+ cameras.

These cameras can be used to do basic gesture recognition. Just by filtering the frames, it is possible to detect a hand and some basic information about it. This starts with simple things such as if it is moving to the left or the right in relation to the camera, or if it is moving closer to or further away from the camera.

Without needing a high level of precision, it is possible to detect these very basic motions and to use them for simple actions. Doing a presentation? Just swipe your hand from left to right over your phone and you're on the next page. Listening to music? Hold your hand over your phone and slowly move it close to turn the volume down.

These gesture are basic, but they are enough to perform a few simple tasks to be done easier, without requiring any special hardware or incurring any cost on the user.


To build this, you will need Qt 5.5+ and OpenCV 3. You can download both of these from their websites and they come with installers. More instructions to follow on how to configure the project for building.
