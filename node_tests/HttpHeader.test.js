const { expect } = require('chai');
const { HttpHeader } = require('../build/Release/addon'); // Adjust the path to your built addon

describe('HttpHeaderWrapper', () => {
    it('should create an instance with key and value', () => {
        const key = 'Content-Type';
        const value = 'application/json';
        const header = new HttpHeader(key, value);

        expect(header).to.be.an('object');
        expect(header.getKey()).to.equal(key);
        expect(header.getValue()).to.equal(value);
    });

    it('should allow changing the key', () => {
        const header = new HttpHeader('Content-Type', 'application/json');
        const newKey = 'Accept';

        header.setKey(newKey);
        expect(header.getKey()).to.equal(newKey);
    });

    it('should allow changing the value', () => {
        const header = new HttpHeader('Content-Type', 'application/json');
        const newValue = 'text/plain';

        header.setValue(newValue);
        expect(header.getValue()).to.equal(newValue);
    });

    it('should return a string representation', () => {
        const key = 'Content-Type';
        const value = 'application/json';
        const header = new HttpHeader(key, value);

        const expectedString = `${key}: ${value}`;
        expect(header.toString()).to.equal(expectedString);
    });
});

