 #include <Utility.h>
 #include<math.h>
 bool withinEpsilon(float val, float epsilon)
 {
    return fabs(val) <= fabs(epsilon);
}