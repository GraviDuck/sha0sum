# sha0sum
A fast, standalone SHA-0 hasher in C that intelligently switches between file-hashing and string-hashing without extra flags.

## 🚀 Smart Input Detection
Unlike standard hash tools, `sha0sum` automatically detects the type of input:

1. **File Mode**: If the argument is a valid file path, it reads the file using a high-speed 32KB buffer (perfect for large files, even 1GB+).
2. **Text Mode**: If no file is found with that name, it treats the argument directly as a raw string and computes its hash.

**No more `--string` or `-s` flags needed.**

