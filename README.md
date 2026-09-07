<img width="1920" height="480" alt="messyfoldericon" src="https://github.com/user-attachments/assets/021c774e-faba-481c-8934-bd35c207114a" />


# Messy Folder
"Give me your messy folders. I'll tell you what's in them, find what's wasting space and clean it up"

A small command-line utility written in C for making chaotic folders slightly less chaotic

## Usage
Compile with `make`  
Then point MessyFolder to a directory e.g. `./messyfolder ~/Downloads`  

### Options
`-h --help` show help  
`-v --version` show version  
`-o --organize` organize files in folders based on extension  
`-d --duplicates` find duplicate files 
`-l --large <MB>` find files larger than the specified size 
<br/>
More features are on the way!

## Roadmap
- [x] Directory scanning
- [x] File organization
- [x] Duplicate detection
- [x] Large file finder
- [ ] Safe duplicate removal
- [ ] Recursive duplicate scanning
- [ ] File name collision handling
- [ ] Messiness score
- [ ] More CLI polish



