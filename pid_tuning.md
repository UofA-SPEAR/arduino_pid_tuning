# Tuning Notes #

Using (https://robotics.stackexchange.com/questions/167/what-are-good-strategies-for-tuning-pid-loops).

Values start at 0:

Values found (1st time):
P: 0.8
I: 0
D: 0

We found that the way the arm was set up mechanically there was enough inherent damping that adding I or D made it less stable than just P.
As well, P can be a large range of values, we just thought 0.8 was nice.
Too much lower than that and it won't start moving or it won't be able to get close to the required value.
Too high and it moves too fast.

Obviously these values can be changed at competition but this seems reasonable.
