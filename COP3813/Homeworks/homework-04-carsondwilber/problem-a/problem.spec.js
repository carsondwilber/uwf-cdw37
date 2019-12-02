//include custom matchers
const styleMatchers = require('jest-style-matchers');
expect.extend(styleMatchers);

//console spy!
const LOG = []; //global to store the logged output
let storeLogFunction = (...inputs) => {
  LOG.push(inputs.reduce((out, arg) => {
    return out+' '+(typeof arg === 'string' ? arg : JSON.stringify(arg));
  },'').trim()); //add it to the log
};
console['log'] = jest.fn(storeLogFunction) //store results of console.log

const jsPath = __dirname + '/index.js';

// Fake timers to help with testing timeouts
jest.useFakeTimers();

describe('Source code is valid', () => {
  test('JavaScript lints without errors', async () => {
    expect([jsPath]).toHaveNoEsLintErrors();
  });
});

const solution = require(jsPath); //load the solution

describe('backpack as JSON', () => {
  test('backpack variable exists', () => {
    expect(JSON.parse(LOG[0])).toEqual({
      color: "blue",
      height: 18,
      width: 12,
      depth: 6,
      contents: ["map", "flashlight", "compass"]});
  });
});


describe('Backpack() constructor creates backpacks', () => {

  test('It returns instance of Backpack', () => {
    expect( new solution.Backpack("gray") )
      .toBeInstanceOf(solution.Backpack);
  });

  test('Backpack objects have expected attributes', () => {
    const pack = new solution.Backpack("gray");
    expect(pack).toHaveProperty('color', 'gray');
    expect(pack).toHaveProperty('height', 18);
    expect(pack).toHaveProperty('width', 12);
    expect(pack).toHaveProperty('depth', 6);
    expect(pack).toHaveProperty('contents', []);
  });
});

describe('packBuilder() builds packs asynchronously', () => {
  test('packBuilder returns a promise', () => {
    expect( solution.packBuilder("red") )
      .resolves.toBeInstanceOf(solution.Backpack);
  });

  test('packBuilder eventually resolves with a Backpack', () => {
    expect( solution.packBuilder("red") )
      .toBeInstanceOf(Promise);
  });
});

describe('scoutTroopPacks() function', () => {
  test('scoutscoutTroopPacks() builds multiple Backpacks', () => {
    solution.scoutTroopPacks().then( packs => {
      expect( Array.isArray(packs) ).toBe(true);
      expect(typeof packs[0]).toBeInstanceOf(solution.Backpack);
    });
  });

  test('Backpacks are consistently created in same order', () => {
    // First try
    solution.scoutTroopPacks().then(packs1 => {
      expect(packs1[0].color).toBe('red');
      expect(packs1[1].color).toBe('blue');
      expect(packs1[2].color).toBe('orange');
      expect(packs1[3].color).toBe('purple');
    });

    // Second try
    solution.scoutTroopPacks().then(packs2 => {
      expect(packs2[0].color).toBe('red');
      expect(packs2[1].color).toBe('blue');
      expect(packs2[2].color).toBe('orange');
      expect(packs2[3].color).toBe('purple');
    });
  });
});