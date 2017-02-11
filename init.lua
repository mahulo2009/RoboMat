wifi.setmode(wifi.STATION)
wifi.sta.config("MOVISTAR_D659","XXX")
print(wifi.sta.getip())

srv=net.createServer(net.TCP)
srv:listen(80,function(conn)
    conn:on("receive", function(client,request)
	print(request) 
	client:send("<h1> Hello, NodeMcu.</h1>")

	client:close();
    collectgarbage();
    end)
end)