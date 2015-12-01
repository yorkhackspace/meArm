This is a description of how to set up L1, L2 and L3 if you use a different sized arm.

The algorithm needs a point of reference (the origin) to refer to things with, which I chose to place directly above the axis the arm rotates around at the base - as this never moves.  Specifically, the point is directly above the base servo, at the same height as the shoulder servo.

From this point, there are four steps (vectors) to get to the gripper:
- Go forward to reach the shoulder (L3a)
- Follow the angle of the upper arm to get to the elbow (L1)
- Follow the angle of the forearm to get to the wrist (L2)
- Go forward to reach the front of the gripper (L3b)

Because the first and last steps are in the same direction, I chose to combine them.  Here's a diagram that might help:

![mearm-geometry](https://cloud.githubusercontent.com/assets/6117158/11512378/b7a06b70-9865-11e5-8bb8-f9adf5963b04.png)

Here's an annotated a photo in case the diagram isn't clear enough (thanks to nihabe).   L3a and L3b are easiest to understand if you look at the arm side-on, as L3 is accounting for extra horizontal distance in the assembly.
There are two rotations happening near the base - one horizontally, to swing the arm left and right (the base pivot axis) and one vertical, to lift and lower the arm (the shoulder pivot axis).  L3a is the front-to-back distance between these - basically, how far forward is the centre of the shoulder servo compared to the centre of the base servo.
At the wrist end, the algorithm has to account for the size of the gripper.  The two interesting points here are the wrist pivot axis (where the arm meets the horizontal "hand" part), and the point where the grips meet when they close (where they'd pick something up).  L3b is the distance between these - basically, how far forward is the point being gripped compared to the wrist.

![1d0f3ccc-97ba-11e5-8b4c-a264fb9dacd7](https://cloud.githubusercontent.com/assets/6117158/11508922/207223e0-9853-11e5-9ac8-e028037a1891.jpg)
