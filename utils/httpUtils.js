const fetch = require('node-fetch');

async function postRequest(url, { data = {}, contentType = 'application/json' } = {}) {
    try {
        const headers = {
        }
        if (contentType) {
            headers['Content-Type'] = contentType;
        }
        const body = JSON.stringify(data);
        const res = await fetch(url, {
            method: 'POST',
            headers,
            body,
        });
        if (res.status !== 200) {
            console.error(res);
            return;
        }
        const responeData = await res.json();
        console.log(responeData);
        return responeData;
    } catch (error) {
        console.error(error);
    }
}

async function getRequest(url, data) {
    try {
        const headers = {
        }
        data = data || {};
        for (const key in data) {
            if (url.includes('?')) {
                url = `${url}&${key}=${encodeURI(data[key])}`;
            } else {
                url = `${url}?${key}=${encodeURI(data[key])}`;
            }
        }
        const res = await fetch(url, {
            method: 'GET',
            headers,
        });
        if (res.status !== 200) {
            console.error(res);
            return;
        }
        const responeData = await res.json();
        // console.log(responeData);
        return responeData;
    } catch (error) {
        console.error(error);
    }
}

module.exports = {
    postRequest,
    getRequest,
};
