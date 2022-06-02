
local blockSize = 12
local encodeSize = 3000 * blockSize

local symbolsTable = {}
for i=48, 122 do
    if (i <= 57) or (i >= 65) and (i <= 90) or (i >= 97) then
        table.insert(symbolsTable, string.char(i))
    end
end

local function lFileDecode(path, password, useRAM)
    if not fileExists(path) then
        return false, "File not found"
    end
    local encodedFile = fileOpen(path)
    if (not encodedFile) then
        return false, "File corrupted"
    end
    local encodedData = fileRead(encodedFile, encodeSize)
    fileSetPos(encodedFile, encodeSize)
    local origData = fileRead(encodedFile, fileGetSize(encodedFile) - encodeSize)
    fileClose(encodedFile)
    local truePassword = string.sub(utf8.lower(md5(password)), 1, 16)
    local block
    local decoded = ""
    for i = 1, encodeSize, blockSize do
        block = teaDecode(string.sub(encodedData, i, i + blockSize), truePassword)
        if type(tonumber(block)) ~= "number" then
            return false, "Wrong password"
        end
        decoded = decoded..string.char(block)
    end
    if (not useRAM) then
        local decodedFileName = ""
        for i = 1, 64 do
            decodedFileName = decodedFileName..symbolsTable[math.random(#symbolsTable)]
        end
        local decodedFile = fileCreate(decodedFileName)
        fileWrite(decodedFile, decoded..origData)
        fileClose(decodedFile)
        return true, string.format(":%s/%s", getResourceName(getThisResource()), decodedFileName)
    else
        return true, decoded..origData
    end
end
local function lSafetyDelete(path)
    local file = fileCreate(path)
    fileWrite(file, "PROTECTED BY XASKEL")
    fileClose(file)
    setTimer(fileDelete, 100, 1, path)
end

-- Экспортные функции
function fileDecode(pt, ps, rm)
    return lFileDecode(pt, ps, rm)
end
function safetyDelete(pt)
    lSafetyDelete(pt)
end

addDebugHook("preFunction", function(_, _, _, _, _, ...) return "skip" end, {"addDebugHook"})
