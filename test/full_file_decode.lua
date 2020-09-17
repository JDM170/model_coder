
local key = "1234" -- Model key
local fileName = "full_file_encode" -- File name

-- Reading crypted file
local file = fileOpen(fileName .. ".enc")
local fileData = fileRead(file, fileGetSize(file))
fileClose(file)

-- Decoding file data
local encodedKey = string.sub(utf8.lower(md5(key)), 1, 16)
local decoded = teaDecode(fileData, encodedKey)

-- Writing uncrypted data to clean file
file = fileCreate(fileName .. ".dec")
fileWrite(file, decoded)
fileClose(file)

encodedKey = nil
fileData = nil
decoded = nil
file = nil
