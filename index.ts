import fs from "node:fs";
import { promisify } from "node:util";

const fsOpen = promisify(fs.open);
const fsRead = promisify(fs.read);
const fsFstat = promisify(fs.fstat);
const fsClose = promisify(fs.close);
const fsWrite = promisify(fs.write);

Uint16Array;

class BinaryReader {}

class ReadonlyBinaryReader {
  private path: string;
  private fileDescriptor: number = 0;
  private openPromise: Promise<void>;

  private position: number = 0;

  constructor(path: string) {
    this.path = path;
    this.openPromise = this.initialOpen();
  }

  private async initialOpen() {
    this.fileDescriptor = await fsOpen(this.path, "r");
  }

  async read(size: number) {
    const buffer = Buffer.alloc(size);
    const bytes = await fsRead(
      this.fileDescriptor,
      buffer,
      0,
      size,
      this.position
    );

    this.position += size;

    return buffer;
  }
}
