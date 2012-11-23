#!/usr/bin/env ruby
require 'socket'

ScreenWidth =  84
ScreenHeight = 48

screen_size = ScreenWidth * ScreenHeight
screen_bytes = screen_size / 8

# Read in options
if ARGV.length != 3
  $stderr.puts "Usage: #{$0} <server_addr> <port> <file_name.dat>"
  exit 1
end
server_addr = ARGV[0]
port = ARGV[1]
file_name = ARGV[2]


# Read the specified input file
puts "Reading [#{file_name}]..."
data = open(file_name, "rb") { |io| io.read }
puts "Read #{data.length} bytes from file."

# Check file is the right size
if data.length != screen_bytes
  $stderr.puts "Error: Unexpected file size. Terminating."
  exit 1
end

# Show the data in hex format
puts "Data as Hex:"
i = 1
data.each_byte do |b|
  print "%02x " % b
  # New line every 24 hex characters
  if i % 24 == 0
    puts
  # Pipe separator every 6 hex character - e.g. one column of pixel data
  elsif i % 6 == 0
    print "| "  if i % 6 == 0
  end
  i+=1 
end

# Show a preview on the terminal
puts "Preview:"
x = 0
y = 0
i = 0
screen = Array.new(screen_size) 
data.each_byte do |b|
  x = i / 6
  y = (i % 6) * 8
  (0..7).each do |o| 
    v = b & (1 << o)
    screen[x + (y * ScreenWidth)] = (v != 0 ? true : false)
    y += 1
  end
  i+=1
end
screen.each_with_index do |b, i|
  print (b ? "o" : " ")
  # Wrap after each row of pixel data
  puts if (i+1) % ScreenWidth == 0
end

# Send the data over UDP/IP
puts "Sending packet to [#{server_addr}:#{port}]..."
s = UDPSocket.new
count = s.send(data, 0, server_addr, port)
puts "#{count} bytes sent."
puts "Done."
