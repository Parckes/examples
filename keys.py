import os.path
import tempfile
import random
import string

class File:
    def __init__ (self, path_to_file):
        self.path_to_file = path_to_file
        if not os.path.exists(path_to_file):
            file = open(f"{path_to_file}", "x")
            file.close()
        self.current = 0

    def read(self):
        with open(f"{self.path_to_file}", "r") as f:
            return f.read()

    def write(self, new_content):
        with open(f"{self.path_to_file}", "w") as f:
            return f.write(new_content)

    def __add__(self, second_file):
        letters = string.ascii_lowercase
        name_of_new_file = ''.join(random.choice(letters) for i in range(10))
        new_path = os.path.join(tempfile.gettempdir(), name_of_new_file)
        new_file = File(new_path)
        text = self.read() + second_file.read()
        new_file.write(text)
        return new_file



    def __str__(self):
        #os.path.abspath("mydir/myfile.txt")
        with open(self.path_to_file) as f:
            return f.name

    def __iter__(self):
        return self

    def __next__(self):
        with open(self.path_to_file) as f:
            for _ in range(self.current):
                f.readline()

            if f.readline() == '':
                self.current = 0;
                raise StopIteration
            else:
                f.seek(0)
            for _ in range(self.current):
                f.readline()

            self.current += 1
            return f.readline()


#file_1 = File("wtf.txt")
#file_2 = File("example.txt")
#file_3 = file_1 + file_2
#print(file_3.read())
