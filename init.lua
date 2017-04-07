wifi.setmode(wifi.STATION)
wifi.sta.config("MOVISTAR_D659","gQK9NJ6amoPbTMfcqz67")
print(wifi.sta.getip())

D1 = 1;
D2 = 2;
D3 = 3;
D4 = 4;

speed=0;
speed1=0;
speed2=0;
wheel=0;
count=204;
direction=0;

pwm.setup(D1,100,speed);
pwm.setup(D2,100,speed);

gpio.mode(D3, gpio.OUTPUT);
gpio.write(D3, gpio.LOW);
gpio.mode(D4, gpio.OUTPUT);
gpio.write(D4, gpio.LOW);


srv=net.createServer(net.TCP)
srv:listen(80,function(conn)
    conn:on("receive", function(sck,payload)

    	print(payload);

		local buf = "";
	    local _, _, method, path, vars = string.find(payload, "([A-Z]+) (.+)?(.+) HTTP");
	    if(method == nil)then
	        _, _, method, path = string.find(payload, "([A-Z]+) (.+) HTTP");
	    end
	    local _GET = {}
	    if (vars ~= nil)then
	        for k, v in string.gmatch(vars, "(%w+)=(%w+)&*") do
	            _GET[k] = v
	        end
	    end

		if(_GET.cmd == "forward" ) then
				speed = tonumber(_GET.param)
				if (speed > 10) then
					speed=10;
				end
				if (speed < 0 ) then
					speed=0
				end
				
				if (speed < 5) then
					speed = 5 - speed;
					direction=1;
				elseif (speed > 5) then
					speed = speed - 5;
					direction=0;
				elseif (speed == 5) then
					speed=0;
					direction=0;
				end
		elseif(_GET.cmd == "backward" ) then
				speed = tonumber(_GET.param)
				if (speed > 5) then
					speed=5;
				end
				direction=1;
		elseif(_GET.cmd == "wheel" ) then
				wheel = tonumber(_GET.param)-3;
				if (wheel > 3) then
					wheel=3;
				end					
		end

		speed1=speed;
		speed2=speed;
			
		if (wheel > 0) then
			speed2 = (speed-wheel);
		elseif (wheel < 0) then
			speed1 = (speed+wheel);
		end

		if (speed1 < 0) then
			speed1=0;
		end

		if (speed2 < 0) then
			speed2=0;
		end

		if (direction == 0) then
			gpio.write(D3, gpio.LOW);
			gpio.write(D4, gpio.LOW);
		else 
			gpio.write(D3, gpio.HIGH);
			gpio.write(D4, gpio.HIGH);
		end

		print("speed1:"..speed1);
		print("speed2:"..speed2);

		pwm.setduty(1,speed1*count);	
		pwm.setduty(2,speed2*count);
		pwm.start(D1);
		pwm.start(D2);

		sck:send("HTTP/1.0 200 OK\r\nAccess-Control-Allow-Origin: *\r\nContent-Length: 25\r\nContent-Type: text/html\r\n\r\n<h1> Hello, NodeMCU.</h1>");

    end)
    conn:on("sent",function(sck)
    	sck:close()
    end)

end)