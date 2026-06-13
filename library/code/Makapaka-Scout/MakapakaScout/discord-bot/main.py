import os
import discord
from discord.ext import commands
from dotenv import load_dotenv
import algorithm as algo_module

load_dotenv()

intents = discord.Intents.default()
intents.message_content = True

bot = commands.Bot(command_prefix='!', intents=intents)

@bot.event
async def on_ready():
    print(f'Logged in as {bot.user}')

@bot.command()
async def ping(ctx):
    await ctx.send('Pong! 🐱')

@bot.command()
async def info(ctx):
    await ctx.send("""
**MakapakaScout** 🐱
Advanced VEX Robotics Scouting & Analytics Platform.

**Features:**
- **MeowScore™:** Comprehensive team rating system (0-100).
- **Match Prediction:** Win probability based on event-weighted stats.
- **TrueSkill:** Match-based skill estimation.

Use `!algorithm` to see how MeowScore is calculated.
""")

@bot.command(aliases=['meowscore'])
async def algorithm(ctx):
    # Discord message limit is 2000 chars, our text is sufficiently short
    await ctx.send(algo_module.ALGORITHM_TEXT)

if __name__ == "__main__":
    token = os.getenv('DISCORD_TOKEN')
    if not token or token == "your_token_here":
        print("Error: DISCORD_TOKEN is not set in .env")
    else:
        bot.run(token)
