#!/usr/bin/ruby
require_relative 'lib/font_table'

ScreenWidth  =  84
ScreenHeight = 48
screen_size  = ScreenWidth * ScreenHeight
screen_bytes = screen_size / 8

char_display = [
  "0123456789abcd",
  "0123456789abcd",
  "0123456789abcd",
  "0123456789abcd",
  "0123456789abcd",
  "0123456789abcd"
]

def put_char(data, char, x, y)
  FontTable[char-0x20].each_with_index do |b, x2|
    #s = "%08b" % b
    #puts s.gsub("0", " ")
    data[36*x + 6*x2 + y ] = b
  end
  #puts
end


data = Array.new(screen_bytes, 0)
char_display.each_with_index do |line, y|
  x = 0
  line.each_byte do |char|
    #puts "(#{x}, #{y})"
    put_char(data, char, x, y)
    x += 1
  end
end

# Show a preview on the terminal
$stderr.puts "Preview:"
x = 0
y = 0
i = 0
screen = Array.new(screen_size)
data.each do |b|
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
  $stderr.print (b ? "o" : " ")
  # Wrap after each row of pixel data
  $stderr.puts if (i+1) % ScreenWidth == 0
end

data.each do |b|
  print b.chr
end

