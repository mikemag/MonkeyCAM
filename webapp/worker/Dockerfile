# Use an LTS version of Node compatible with the worker's minimum requirement
FROM node:20-slim

# Create app directory
WORKDIR /app

# Install production dependencies
COPY package*.json ./
RUN npm ci --omit=dev

# Copy the worker source
COPY . ./

# Configure runtime defaults for Cloud Run
ENV NODE_ENV=production \
    PORT=8080 \
    monkeycamPath=/app/bin/linux

# Ensure the MonkeyCAM binary is executable when present
RUN /app/bin/linux/MonkeyCAM --version

EXPOSE 8080
CMD ["npm", "start"]
