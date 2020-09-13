
-- Opening crypted file
local file = fileOpen("test.txd.enc")
local fileData = fileRead(file, fileGetSize(file))
fileClose(file)

-- Decoding file data
local decoded = teaDecode(fileData, "JdzFR2XLDaBtpGGD")
fileData = nil

-- Writing uncrypted data to clean file
file = fileCreate("test.txd.dec")
fileWrite(file, decoded)
fileClose(file)

decoded = nil
file = nil
