import sys

sourceFile = "index.htm"
outFile = sourceFile + ".string"

def createString():
    data = []
    with open(sourceFile, "r") as fi:
        for line in fi:            
            data.append(line)
        
    with open(outFile, "w") as fo:
        for line in data:
            stripped = line.replace('\n','').replace('"','\\"') 
            if "//" in stripped:
                stripped = stripped[:stripped.find("//")]
            fo.write('"{}"\n'.format(stripped))            

createString()