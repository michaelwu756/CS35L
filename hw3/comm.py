#!/usr/bin/python

import sys
from optparse import OptionParser

class comm:
    def __init__(self, file1, file2):
        if file1=="-":
            self.lines = sys.stdin.readlines()
        else:
            try:
                f = open(file1, "r")
                self.lines = f.readlines()
                f.close()
            except (FileNotFoundError, PermissionError) as e:
                sys.stderr.write("comm: "+file1+": "+e.args[1]+"\n")
                sys.exit(1)
        if file2=="-":
            self.lines2 = sys.stdin.readlines()
        else:
            try:
                f2 = open(file2, "r")
                self.lines2 = f2.readlines()
                f2.close()
            except (FileNotFoundError, PermissionError) as e:
                sys.stderr.write("comm: "+file1+": "+e.args[1]+"\n")
                sys.exit(1)
        for x in range(len(self.lines)):
            if self.lines[x].endswith("\n"):
                self.lines[x]=self.lines[x][:-1]

        for x in range(len(self.lines2)):
            if self.lines2[x].endswith("\n"):
                self.lines2[x]=self.lines2[x][:-1]
            
        
    def printInCol(self, str, num, sList):
        if sList[num-1]==False:
            for count in range(1, 4):
                if sList[count-1]==False and sList[num-1]==False:
                    if count<num:
                        sys.stdout.write("\t")
                    if count==num:
                        sys.stdout.write(str+"\n")

    def printComparisons(self, s1, s2, s3):
        x=0
        y=0
        sList=[s1,s2,s3]
        while x<len(self.lines) and y<len(self.lines2):
            if self.lines[x]<self.lines2[y]:
                self.printInCol(self.lines[x],1,sList)
                x+=1
            elif self.lines[x]>self.lines2[y]:
                self.printInCol(self.lines2[y],2,sList)
                y+=1
            elif self.lines[x]==self.lines2[y]:
                self.printInCol(self.lines[x],3,sList)
                x+=1
                y+=1
        
        while x<len(self.lines):
            self.printInCol(self.lines[x],1,sList)
            x+=1
        while y<len(self.lines2):
            self.printInCol(self.lines2[y],2,sList)
            y+=1
                                                            
                                    
    def printComparisonsUnsorted(self, s1, s2, s3):
        sList=[s1,s2,s3]
        for str1 in self.lines:
            matched=False
            for str2 in self.lines2:
                if str1==str2:
                    matched=True
                    self.printInCol(str1,3,sList)
                    self.lines2.remove(str2)
                    break
            if matched==False:
                self.printInCol(str1,1,sList)
                
        for str2 in self.lines2:
            self.printInCol(str2,2,sList)         
                                
def main():
    version_msg = "%prog by Michael Wu"
    usage_msg = """%prog [OPTION] FILE1 FILE2
Compare sorted files FILE1 and FILE2 line by line.

With no options, produce three-column output.  Column one contains
lines unique to FILE1, column two contains lines unique to FILE2,
and column three contains lines common to both files.

The -u flag changes this command to go through each line in FILE1
and checks if there is a matching line in FILE2, even if both files
are unsorted. Then it prints the lines in FILE1 in the appropriate
columns, and prints the remaining lines in FILE2 that were not
matched."""
    
    parser = OptionParser(version=version_msg, usage=usage_msg)
    parser.set_defaults(supress1=False, supress2=False, supress3=False,
                        unsorted=False)
    parser.add_option("-1", action="store_true", dest="supress1",
                      help="suppress column 1 (lines unique to FILE1)")
    parser.add_option("-2", action="store_true", dest="supress2",
                      help="suppress column 2 (lines unique to FILE2)")
    parser.add_option("-3", action="store_true", dest="supress3",
                      help=("suppress column 3 (lines that appear in both"+
                            " files)"))
    parser.add_option("-u", action="store_true", dest="unsorted",
                      help="checks for matching lines in unsorted files")
    
    options, args = parser.parse_args(sys.argv[1:])
    
    if len(args)==0:
        sys.stderr.write("comm: missing operand\n")
        sys.stderr.write("Try 'comm --help' for more information.\n")
        sys.exit(1);
    if len(args)==1:
        sys.stderr.write("comm: missing operand after '"+args[0]+"'\n")
        sys.stderr.write("Try 'comm --help' for more information.\n")
        sys.exit(1);
        
    commObj= comm(args[0],args[1])
    if options.unsorted==True:
        commObj.printComparisonsUnsorted(options.supress1,
                                         options.supress2,
                                         options.supress3)
    else:
        commObj.printComparisons(options.supress1,
                                 options.supress2,
                                 options.supress3)
if __name__ == "__main__":
    main()
