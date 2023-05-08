def read_file_clang(file_path):
    with open(file_path, 'r') as file:
        lines = file.readlines()

    diagnostic_names = []

    for line in lines:
        if "DiagnosticName: " in line:
            diagnostic_name = line.split("DiagnosticName: ")[1].split("\n")[0]
            diagnostic_names.append(diagnostic_name)

    return list(dict.fromkeys(diagnostic_names))

def read_file_cppcheck(file_path):
    with open(file_path, 'r') as file:
        lines = file.readlines()

    diagnostic_names = []

    for line in lines:
        if "id=" in line:
            diagnostic_name = line.split("id=")[1].split(" ")[0]
            diagnostic_names.append(diagnostic_name)

    return list(dict.fromkeys(diagnostic_names))

BasicLibPathClang = 'StaticAnalysisLogs/BasicLib/clang-tidy.yml'
BasicLibPathCppcheck = 'StaticAnalysisLogs/BasicLib/cppcheck.xml'

SimpleMUDPathClang = 'StaticAnalysisLogs/SimpleMUD/clang-tidy.yml'
SimpleMUDPathCppcheck = 'StaticAnalysisLogs/SimpleMUD/cppcheck.xml'

SocketLibPathClang = 'StaticAnalysisLogs/SocketLib/clang-tidy.yml'
SocketLibPathCppcheck = 'StaticAnalysisLogs/SocketLib/cppcheck.xml'

ThreadLibPathClang = 'StaticAnalysisLogs/ThreadLib/clang-tidy.yml'
ThreadLibPathCppcheck = 'StaticAnalysisLogs/ThreadLib/cppcheck.xml'

clang = []
clang.extend(read_file_clang(BasicLibPathClang))
clang.extend(read_file_clang(SimpleMUDPathClang))
clang.extend(read_file_clang(SocketLibPathClang))
clang.extend(read_file_clang(ThreadLibPathClang))
clang = list(dict.fromkeys(clang))
clang.sort()

cppcheck = []
cppcheck.extend(read_file_cppcheck(BasicLibPathCppcheck))
cppcheck.extend(read_file_cppcheck(SimpleMUDPathCppcheck))
cppcheck.extend(read_file_cppcheck(SocketLibPathCppcheck))
cppcheck.extend(read_file_cppcheck(ThreadLibPathCppcheck))
cppcheck = list(dict.fromkeys(cppcheck))
cppcheck.sort()

for i in cppcheck:
    print('|', i, '| cppcheck | warning | | | | |')
print()
for i in clang:
    print('|', i, '| clang-tidy | warning | | | |')
