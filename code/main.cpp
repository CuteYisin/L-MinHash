#include <bits/stdc++.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
using namespace std;


class Timer {
    private:
        string title;
        chrono::high_resolution_clock::time_point st, ed;

    public:
        Timer(const string& title): title(title) {
            cerr << "+++ " << title << " timer started ... " << endl;
            st = chrono::high_resolution_clock::now();
        }

        ~Timer() {
            stop();
        }

        void stop() {
            ed = chrono::high_resolution_clock::now();
            chrono::milliseconds ms = chrono::duration_cast<chrono::milliseconds>(ed - st);
            cerr << "+++ " << title << " used " << (ms.count() * 0.001) << "s +++" << endl;
        }
};



class Sequence {
    public:
        string name;
        string seq;

        Sequence(const string& name, const string& seq): name(name), seq(seq) {}

        ~Sequence() {}

        void show() {
            cout << name << endl;
            cout << seq << endl;
        }
};



class Sequence_sv {
    public:
        string_view name;
        string_view seq;

        Sequence_sv(const string_view& name, const string_view& seq): name(name), seq(seq) {}

        ~Sequence_sv() {}

        void show() {
            cout << name << endl;
            cout << seq << endl;
        }
};



class Data {
    private:
        string title;
        string filelistname;
        string datadir;
        vector <string> filelist;
        ifstream ifs;

    public:
        vector <Sequence> data;
        vector <Sequence_sv> data_sv;

        Data(const string& title, const string& filelistname, const string& datadir): title(title), filelistname(filelistname), datadir(datadir) {
            cerr << "\n=== Load " << title << " data ===" << endl;
            Timer timer(title);
            load_list();
            load_files();
            //load_files_fast();
            cerr << "=== Finish Loading! ===" << endl;
        }

        ~Data() {}

        void load_list() {
            cerr << "--- Load " << title << " file list from \"" << filelistname << "\" ..." << endl;
            ifs.open(filelistname);
            string filename;
            while(ifs >> filename) {
                filelist.push_back(filename);
            }
            ifs.close();
        }

        void load_files() {
            cerr << "--- Load " << title << " sequences from " << filelist.size() << " files ..." << endl;
            for(string filename: filelist) {
                filename = datadir + filename;
                cerr << "--- Load from file \"" << filename << "\" ..." << endl;
                ifs.open(filename);
                string name, seq;
                while(ifs >> name >> seq) {
                    data.push_back(Sequence(name.c_str(), seq.c_str()));
                }
                ifs.close();
            }
            cerr << "--- " << data.size() << " sequences loaded ..." << endl;
            for(int i = 0; i < 3; i ++) {
                data[i].show();
            }
            for(int i = 0; i < 3; i ++) {
                data[data.size() - 1 - i].show();
            }
        }

        void load_files_fast() {
            cerr << "--- (fast version) Load " << title << " sequences from " << filelist.size() << " files ..." << endl;
            for(string filename: filelist) {
                filename = datadir + filename;
                cerr << "--- (fast version) Load from file \"" << filename << "\" ..." << endl;
                int fd = open(filename.c_str(), O_RDONLY);
                struct stat statbuf;
                fstat(fd, &statbuf);
                const char* ptr = static_cast <const char*> (mmap(NULL, statbuf.st_size, PROT_READ, MAP_PRIVATE, fd, 0));

                size_t res = statbuf.st_size;
                string_view name, seq;
                const char* p = ptr;
                const char* q = NULL;
                int length;
                int n = count(ptr, ptr + statbuf.st_size, '\n') / 2;
                cout << n << endl;
                for(int i = 0; i < n; i ++) {
                    q = static_cast <const char*> (memchr(p, '\n', res));
                    length = static_cast <int> (q - p);
                    name = {p, length};
                    p = q + 1, res -= length;
                    q = static_cast <const char*> (memchr(p, '\n', res));
                    length = static_cast <int> (q - p);
                    seq = {p, length};
                    p = q + 1, res -= length;
                    data_sv.push_back(Sequence_sv(name, seq));
                }

            }
            cerr << "--- (fast version) " << data_sv.size() << " sequences loaded ..." << endl;
            for(int i = 0; i < 3; i ++) {
                data_sv[i].show();
            }
            for(int i = 0; i < 3; i ++) {
                data_sv[data_sv.size() - 1 - i].show();
            }
        }
};



class Bruteforce {

};



int main() {
    cerr << "\n=== Start! ===" << endl;
    Data queries("Query", "./query.in", "../data/");
    Data database("Database", "./database.in", "../data/");
    cerr << "=== All done! ===" << endl;
    return 0;
}
