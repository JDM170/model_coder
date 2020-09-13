
local key = "1234" -- Model key
local fileName = "partial_file_encode" -- File name
local encodeSize = 36000 -- Encoding size (3000*12)

-- Reading crypted file
local file = fileOpen(fileName .. ".enc")
local encodedFileData = fileRead(file, encodeSize)
fileSetPos(file, encodeSize)
local originalFileData = fileRead(file, fileGetSize(file) - encodeSize)
fileClose(file)

-- Decoding file data
local block = ""
local decoded = ""
local encodedKey = string.sub(utf8.lower(md5(key)), 1, 16)
for i = 1, encodeSize, 12 do
	block = teaDecode(string.sub(encodedFileData, i, i + 12), encodedKey)
	decoded = decoded .. block
end

-- Writing uncrypted data to clean file
file = fileCreate(fileName .. ".dec")
fileWrite(file, decoded .. originalFileData)
fileClose(file)

originalFileData = nil
encodedFileData = nil
encodedKey = nil
decoded = nil
block = nil
file = nil
