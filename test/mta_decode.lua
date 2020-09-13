
local key = "JdzFR2XLDaBtpGGD" -- Model key
local fileName = "test.txd" -- File name

-- Opening crypted file
local file = fileOpen(fileName..".enc")
local fileData = fileRead(file, fileGetSize(file))
fileClose(file)

-- Decoding file data
local decoded = teaDecode(fileData, string.sub(utf8.lower(md5(key)), 1, 16))
fileData = nil

-- Writing uncrypted data to clean file
file = fileCreate(fileName..".dec")
fileWrite(file, decoded)
fileClose(file)

decoded = nil
file = nil
