// Require the Axios module.
const axios = require('axios');

var decade = null;

// Define API middleware.
let tvService = {};

// Define search function.
tvService.searchTitle = async function (query)
{
    if (query.decade && parseInt(query.decade))
    {
        decade = parseInt(query.decade);
    }

    return axios.get('http://api.tvmaze.com/search/shows?q=' + query.terms.replace(/\s/g, '+'))
        .then(function (response) {
            let finalResult = [];

            for (idx in response.data)
            {
                if (response.data[idx].show.status != 'In Development')
                {
                    if (matchActiveFilter(response.data[idx]))
                    {
                        finalResult.push(response.data[idx]);
                    }
                }
            }

            return {data: finalResult, filter: decade};
        })
        .catch(function (error) {
            console.log(error.message);
            return {};
        });
}

tvService.fetchDetails = function (showId) {
    return axios.get('http://api.tvmaze.com/shows/' + showId.showId)
        .then(function (result) {
            return result.data;
        })
        .catch(function (error) {
            console.log(error.message);
            return {};
        });
};

tvService.clearFilter = function () {
    return decade = null;
};

// Define search filter by decade.
function matchActiveFilter(item)
{
    if (decade && item.show.premiered)
    {
        let itemDecade = parseInt(item.show.premiered.slice(0, 4));

        return (itemDecade > decade && itemDecade < decade + 10);
    }

    return true;
}

module.exports = tvService;