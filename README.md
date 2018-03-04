# tapCombiner
A project to solve private task - to combine several .tap files into one .cnc master file following some rules: 
1. We write header lines of the first file only (several first lines)
2. We write footer lines of the last file only (several last lines)
3. After header lines, we paste an injection text
4. Starting at second file, we begin writing separators before body of each file (E.g. #start2;)

Project seems finished, but dependencies are strong to the local machine use.
It uses pugixml 1.8.0 for config file management (it stores size of header and footer, injection and separator texts)
It also uses boost::filesystem to iterate through files passed via path directory.
