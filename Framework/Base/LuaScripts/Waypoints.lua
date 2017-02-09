Point1 = { x = -50, y = 1 , z = -50 }
Point2 = { x = 50, y = 1 , z = 50 }
Point3 = { x = -50, y = 1 , z = 50 }
Point4 = { x = 50, y =  1, z = -50 }

math.randomseed(os.time())
function GenerateWayPoint(center, range)
   print("randomizing Waypoint Vector")
   local upper = center + range
   local lower = center - range
   local x = math.random(lower, upper)
    -- % (range*2) + 1 - range + center
   local y = 1
   local z = math.random(lower, upper) 
   -- % (range*2) + 1 - range + center
   return x,y,z
end