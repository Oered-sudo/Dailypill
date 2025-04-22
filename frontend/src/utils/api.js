import axios from 'axios';

const API_URL = 'http://localhost:5000/api/alarm'; // Change this to your backend URL

export const fetchAlarms = async () => {
    try {
        const response = await axios.get(API_URL);
        return response.data;
    } catch (error) {
        console.error('Error fetching alarms:', error);
        throw error;
    }
};

export const createAlarm = async (alarmData) => {
    try {
        const response = await axios.post(API_URL, alarmData);
        return response.data;
    } catch (error) {
        console.error('Error creating alarm:', error);
        throw error;
    }
};

export const updateAlarm = async (id, alarmData) => {
    try {
        const response = await axios.put(`${API_URL}/${id}`, alarmData);
        return response.data;
    } catch (error) {
        console.error('Error updating alarm:', error);
        throw error;
    }
};

export const deleteAlarm = async (id) => {
    try {
        await axios.delete(`${API_URL}/${id}`);
    } catch (error) {
        console.error('Error deleting alarm:', error);
        throw error;
    }
};