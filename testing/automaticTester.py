import os
import subprocess
import sys
import time

def main():
    currPath = os.path.dirname(os.path.realpath(__file__))
    cTime = str(int(time.time()))

    name = input("Enter the name of the executable file (without extension): ")
    #options = input("Specify extra command line options for compile (with \"-\"): ")
    #os.system("gcc -o " + currPath + "/" + name + " " + currPath + "/" + name + ".c " + options)

    try:
        os.mkdir(currPath + "/sets/")
        os.mkdir(currPath + "/results/")
        os.mkdir(currPath + "/publicTests/")
        print("Place test sets in folder \"sets\" and restart.")
        exit()
    except(FileExistsError):
        pass

    try:
        os.mkdir(currPath + "/results/")
    except(FileExistsError):
        pass

    sets=os.listdir(currPath + "/sets/")

    for s in sets:
        if(s[0]=="."):
            continue
        try:
            fl=open(currPath + "/results/times" + cTime + ".txt", "w")
            fl.close()
        except(FileNotFoundError):
            print("Please create a directory named \"results\" in the same directory of the script and DON'T remove it, restart to automatically add it")
            sys.exit(1)
        levels=os.listdir(currPath + "/sets/"+s+"/")
        for l in levels:
            if(l[0]=="." or l[0]=="R"):
                continue
            if l!="README.txt":
                files=os.listdir(currPath + "/sets/"+s+"/"+l+"/")
                for f in files:
                    if(f[0]=="t"):
                        fl=open(currPath + "/sets/"+s+"/"+l+"/"+f, "r")
                        lines=fl.readlines()
                        fl.close()
                        fl=open(currPath + "/sets/"+s+"/"+l+"/"+f+".in", "w")
                        fl.writelines(lines)
                        fl.close()
                        fl=open(currPath + "/sets/"+s+"/"+l+"/"+f+".in", "r")
                        start = time.perf_counter()
                        stream = subprocess.Popen([currPath + "/" + name], stdin=fl,  stdout=subprocess.PIPE, shell=True)
                        #stream.wait()
                        finish = time.perf_counter()
                        fl.close()
                        #os.remove(currPath + "/sets/"+s+"/"+l+"/"+f+".in")
                        results=list()
                        #resultsEncoded=stream.communicate()[0]
                        (resultsEncoded, err) = stream.communicate()
                        os.remove(currPath + "/sets/"+s+"/"+l+"/"+f+".in")
                        results = resultsEncoded.decode().split("\n")
                        # for re in resultsEncoded.decode():
                        #     pass
                        #     #results.append((bytes(re)).decode("ascii"))
                        fl=open(currPath + "/sets/"+s+"/"+l+"/sol"+f.replace("test", "").replace(".txt", "")+".txt")
                        solution=fl.readlines()
                        fl.close()

                        flag=False
                        for x in range(len(solution)):
                            flag=False
                            try:
                                if(solution[x].replace("\n", "")==results[x].replace("\n", "")):
                                    pass
                                else:
                                    flag=True
                                    # print("Error found! Set: " + s + ", test: " + f)
                                    # sys.exit(1)
                            except:
                                flag=True
                                # print("Error found!")
                                # exit()
                            if(flag):
                                print("Error found! Set: " + s + ", level: " + l.replace("level", "") + ", test: " + f.replace("test", "").replace(".txt", ""))
                                fl=open(currPath + "/results/times" + cTime + ".txt", "a")
                                fl.write(s + " - " + l + " - " + f.replace(".txt", "") + " || Test not passed! Execution time: " + str(round((finish-start)*1000, 3)) + " ms\n")
                                fl.close()
                                break
                                #sys.exit(1)
                        if(not flag):
                            fl=open(currPath + "/results/times" + cTime + ".txt", "a")
                            fl.write(s + " - " + l + " - " + f.replace(".txt", "") + " || Test passed! Execution time: " + str(round((finish-start)*1000, 3)) + " ms\n")
                            fl.close()

    ######## PUBLIC TESTS

    try:
        sets=os.listdir(currPath + "/publicTests/")
    except(FileNotFoundError):
        print("Please create a folder named \"publicTests\" and DON'T remove it, restart to automatically add it.")

    for s in sets:
        levels=os.listdir(currPath + "/publicTests/"+s+"/")
        for l in levels:
            if "input" in l:
                fl=open(currPath + "/publicTests/"+s+"/"+l, "r")
                lines=fl.readlines()
                fl.close()
                fl=open(currPath + "/publicTests/"+s+"/"+l.replace(".txt", ".in"), "w")
                fl.writelines(lines)
                fl.close()
                fl=open(currPath + "/publicTests/"+s+"/"+l.replace(".txt", ".in"), "r")
                start = time.perf_counter()
                stream = subprocess.Popen([currPath + "/" + name], stdin=fl,  stdout=subprocess.PIPE, shell=True)
                #stream.wait()
                finish = time.perf_counter()
                fl.close()
                results=list()
                (resultsEncoded, err) = stream.communicate()
                os.remove(currPath + "/publicTests/"+s+"/"+l.replace(".txt", ".in"))
                results = resultsEncoded.decode().split("\n")
                fl=open(currPath + "/publicTests/"+s+"/"+l.replace("_input", "_output"))
                solution=fl.readlines()
                fl.close()

                flag=False
                for x in range(len(solution)):
                    flag=False
                    try:
                        if(solution[x].replace("\n", "")==results[x].replace("\n", "")):
                            pass
                        else:
                            flag=True
                            # print("Error found! Set: " + s + ", test: " + f)
                            # sys.exit(1)
                    except:
                        flag=True
                        # print("Error found!")
                        # exit()
                    if(flag):
                        print("Error found! Test: " + l.replace("_input", "").replace(".txt", ""))
                        fl=open(currPath + "/results/times" + cTime + ".txt", "a")
                        fl.write(l.replace("_input", "").replace(".txt", "") + " || Test not passed! Execution time: " + str(round((finish-start)*1000, 3)) + " ms\n")
                        fl.close()
                        break
                        #sys.exit(1)
                if(not flag):
                    fl=open(currPath + "/results/times" + cTime + ".txt", "a")
                    fl.write(l.replace("_input", "").replace(".txt", "") + " || Test passed! Execution time: " + str(round((finish-start)*1000, 3)) + " ms\n")
                    fl.close()



    #####################

    print("Finished testing!")
    sys.exit(1)


if __name__ == "__main__":
    main()