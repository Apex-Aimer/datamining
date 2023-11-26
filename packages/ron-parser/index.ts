import { isSeparationSymbol } from "./isSeparationSymbol";
import { FromSchema, JSONSchema } from "json-schema-to-ts";
import { Validator } from "jsonschema";

// @ts-expect-error
type RON_t = Record<string, string | RON_t>;

export class RON {
  private objectsStack: RON_t[] = [{}];
  private literalsStack: string[] = [];

  private processLiteral(numOfLiterals: number) {
    if (numOfLiterals < 2) {
      return false;
    }

    // console.log(this.literalsStack);

    const value = this.literalsStack.pop();
    const property = this.literalsStack.pop();

    // console.log({ property, value });

    this.objectsStack[this.objectsStack.length - 1][property] = value;

    return true;
  }

  private async parseLine(line: string) {
    const chars = line.split("");

    let isQuoted = false;
    let isInLiteral = false;

    let numOfLiterals = 0;

    if (line.startsWith("{")) {
      this.objectsStack.push({});

      return;
    }

    if (line.includes("{}")) {
      return;
    }

    if (line.includes("{")) {
      this.objectsStack.push({});
      const key = line.substring(0, line.indexOf("{"));

      if (key.length > 0) {
        this.literalsStack.push(key);
      }

      return;
    }

    for (const char of chars) {
      if (!isInLiteral) {
        if (char === "}") {
          const obj = this.objectsStack.pop() as Record<string, string>;
          const objProperty = this.literalsStack.pop() as string;

          this.objectsStack[this.objectsStack.length - 1][objProperty] = obj;

          return;
        }

        if (isSeparationSymbol(char)) {
          continue;
        }

        if (char === "'" || char === '"') {
          this.literalsStack.push("");
          isInLiteral = true;
          isQuoted = true;
          continue;
        }

        this.literalsStack.push(char);
        isInLiteral = true;
        continue;
      }
      if (isQuoted) {
        if (char === "'" || char === '"') {
          isInLiteral = false;
          isQuoted = false;
          if (this.processLiteral(++numOfLiterals)) {
            return;
          }
          continue;
        }

        this.literalsStack[this.literalsStack.length - 1] =
          this.literalsStack[this.literalsStack.length - 1] + char;

        continue;
      }
      if (isSeparationSymbol(char)) {
        isInLiteral = false;
        if (this.processLiteral(++numOfLiterals)) {
          return;
        }
        continue;
      }

      this.literalsStack[this.literalsStack.length - 1] =
        this.literalsStack[this.literalsStack.length - 1] + char;
    }

    if (isInLiteral) {
      this.processLiteral(++numOfLiterals);
    }
  }

  // @ts-ignore
  async parse<T extends JSONSchema>(
    file: string,
    schema: T
  ): Promise<FromSchema<T>> {
    const lines = file.split("\n");

    for (const line of lines) {
      const normalizedLine = line.trim().replace(/(^[^\/]+)(\/\/.*)$/gi, "$1");

      if (normalizedLine.length === 0) {
        continue;
      }

      if (normalizedLine.startsWith("//")) {
        continue;
      }

      await this.parseLine(normalizedLine);
    }

    const obj = this.objectsStack[0];

    try {
      new Validator().validate(obj, schema as any, {
        throwError: true,
      });
    } catch (err) {
      console.log(obj);
      throw err;
    }

    return obj;
  }
}
