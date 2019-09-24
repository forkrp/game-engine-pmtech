import collections
import sys
import os.path
import json
import fnmatch
import util
import jsn.jsn as jsn


# premake
def run_premake(config):
    pass


# pmfx
def run_pmfx(config):
    pass


# models
def run_models(config):
    pass


# textures
def run_textures(config):
    pass


# copy
def run_copy(config):
    print("--------------------------------------------------------------------------------")
    print("copy ---------------------------------------------------------------------------")
    print("--------------------------------------------------------------------------------")
    copy_tasks = config["copy"]
    for task in copy_tasks:
        if len(task) != 2:
            print("[error] copy tasks must be an array of size 2 [src, dst]")
            exit(1)
        fn = task[0].find("*")
        if fn != -1:
            # wildcards
            fnroot = task[0][:fn-1]
            for root, dirs, files in os.walk(fnroot):
                for file in files:
                    src = util.sanitize_file_path(os.path.join(root, file))
                    if fnmatch.fnmatch(src, task[0]):
                        dst = src.replace(util.sanitize_file_path(fnroot), util.sanitize_file_path(task[1]))
                        util.copy_file_create_dir_if_newer(src, dst)
        elif os.path.isdir(task[0]):
            # dir
            for root, dirs, files in os.walk(task[0]):
                for file in files:
                    src = util.sanitize_file_path(os.path.join(root, file))
                    dst = src.replace(util.sanitize_file_path(task[0]), util.sanitize_file_path(task[1]))
                    util.copy_file_create_dir_if_newer(src, dst)
        else:
            # single file
            util.copy_file_create_dir_if_newer(task[0], task[1])


# entry point of pmbuild
if __name__ == "__main__":
    print("--------------------------------------------------------------------------------")
    print("pmbuild (v3) -------------------------------------------------------------------")
    print("--------------------------------------------------------------------------------")
    config_all = jsn.loads(open("config.jsn", "r").read())
    print(json.dumps(config_all, indent=4))

    # tasks are executed in order they are declared
    tasks = collections.OrderedDict()
    tasks["premake"] = run_premake
    tasks["pmfx"] = run_pmfx
    tasks["models"] = run_models
    tasks["textures"] = run_textures
    tasks["copy"] = run_copy

    # first arg is build profile
    config = config_all[sys.argv[1]]

    # run tasks
    for key in tasks.keys():
        if key not in config.keys():
            continue
        if "-all" in sys.argv and "-n" + key not in sys.argv:
            tasks.get(key, lambda config: '')(config)
        elif len(sys.argv) != 2 and "-" + key in sys.argv:
            tasks.get(key, lambda config: '')(config)
        elif len(sys.argv) == 2:
            tasks.get(key, lambda config: '')(config)