import os

path = os.path.abspath(os.path.join(os.getcwd(), '../Src'))
fileCount = 0
lineCount = 0
for roots, dirs, files in os.walk(path):
    for file in files:
        # print(file)
        fileCount += 1
        fp = open(os.path.join(roots, file), 'rb')
        for line in fp:
            lineCount += 1

print("Total file:", fileCount)
print("Total lines of all code:", lineCount)
